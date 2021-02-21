export interface Friend {
  id: number;
  name: string;
  avatar: string;
  online: boolean;
}

/** Player status. */
export class Player implements Friend {
  /** Player ID. */
  id: number;

  /** Server id player. */
  serverId: number;

  /** Username. */
  name: string;

  avatar: string;

  /** Current health. */
  health?: number;

  /** Level */
  level?: number;

  /** Online status. */
  online: boolean;

  /** Connected in server. */
  connected: boolean;

  /** Token Invitation. */
  invitation: string;

  /** invitation sent. */
  invitationSent: boolean;

  /** CellName */
  cellName: string;

  /** isLoaded */
  isLoaded: boolean;

  constructor(options: {
    id?: number,
    serverId?: number,
    name?: string,
    avatar?: string,
    online?: boolean,
    connected?: boolean,
    health?: number,
    level?: number,
    invitation?: string,
    invitationSent?: boolean,
    cellName?: string,
    isLoaded?: boolean
  } = {}) {
    this.id = options.id || 0;
    this.serverId = options.serverId || 0;
    this.name = options.name || '';
    this.avatar = options.avatar || '';
    this.invitation = options.invitation || '';
    this.cellName = options.cellName || 'vide';

    if (options.health || options.health === 0) {
      this.health = options.health;
    }
    else {
      this.health = undefined;
    }

    if (options.level || options.level === 0) {
      this.level = options.level;
    }
    else {
      this.level = undefined;
    }

    if (options.online === undefined || options.online === null) {
      this.online = false;
    }
    else {
      this.online = options.online;
    }

    if (options.connected === undefined || options.connected === null) {
      this.connected = false;
    }
    else {
      this.connected = options.connected;
    }

    if (options.invitationSent === undefined || options.invitationSent === null) {
      this.invitationSent = false;
    }
    else {
      this.invitationSent = options.invitationSent;
    }

    if (options.isLoaded === undefined || options.isLoaded === null) {
      this.isLoaded = false;
    }
    else {
      this.isLoaded = options.isLoaded;
    }
  }
}
